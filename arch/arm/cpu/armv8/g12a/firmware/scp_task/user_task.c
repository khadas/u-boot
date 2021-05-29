/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/g12a/firmware/scp_task/user_task.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "config.h"
#include "data.h"
#include "registers.h"
#include "task_apis.h"
#include "suspend.h"
#include <amlogic/aml_cec.h>

#define TASK_ID_IDLE 0
#define TASK_ID_LOW_MB	3
#define TASK_ID_HIGH_MB	4
#define TASK_ID_SECURE_MB  5

enum scpi_client_id {
	SCPI_CL_NONE,
	SCPI_CL_CLOCKS,
	SCPI_CL_DVFS,
	SCPI_CL_POWER,
	SCPI_CL_THERMAL,
	SCPI_CL_REMOTE,
	SCPI_CL_LED_TIMER,
	SCPI_CL_SET_CEC_DATA,
	SCPI_CL_UPDATE_PHYADDR,
	SCPI_MAX,
};

void __switch_idle_task(void)
{
	register int p0 asm("r0") = 2;
	register int p1 asm("r1") = TASK_ID_IDLE;

	asm("svc 0" :  : "r"(p0), "r"(p1));
}
void __switch_back_securemb(void)
{
	register int p0 asm("r0") = 2;
	register int p1 asm("r1") = TASK_ID_SECURE_MB;

	asm("svc 0" :  : "r"(p0), "r"(p1));
}
void __switch_back_highmb(void)
{
	register int p0 asm("r0") = 2;
	register int p1 asm("r1") = TASK_ID_HIGH_MB;

	asm("svc 0" :  : "r"(p0), "r"(p1));
}

void __switch_back_lowmb(void)
{
	register int p0 asm("r0") = 2;
	register int p1 asm("r1") = TASK_ID_LOW_MB;

	asm("svc 0" :  : "r"(p0), "r"(p1));
}

void secure_task(void)
{
	volatile unsigned *pcommand =
	    (unsigned *)(&(secure_task_share_mem[TASK_COMMAND_OFFSET]));
	volatile unsigned *response =
	    (unsigned *)(&(secure_task_share_mem[TASK_RESPONSE_OFFSET]));
	unsigned command;
	struct resume_param *presume;
	unsigned int state;

	/*init bss */
	bss_init();
	dbg_prints("secure task start!\n");

	/* suspend pwr ops init*/
	suspend_pwr_ops_init();
	*pcommand = 0;

	while (1) {
		/* do secure task process */
		command = *pcommand;
		if (command) {
			dbg_print("process command ", command);
			if (command == SEC_TASK_GET_WAKEUP_SRC) {
				state = *(pcommand+1);
				suspend_get_wakeup_source(
						(void *)response,  state);
			} else if (command == COMMAND_SUSPEND_ENTER) {
				state = *(pcommand+1);
				enter_suspend(state);
				*pcommand = 0;
				*response = RESPONSE_SUSPEND_LEAVE;
				presume = (struct resume_param *)(response+1);
				presume->method = resume_data.method;
				if (presume->method == CEC_WAKEUP) {
					presume->date1 = resume_data.date1;
					presume->date2 = resume_data.date2;
					memcpy(presume->cec_otp_msg,
					       resume_data.cec_otp_msg,
					       sizeof(resume_data.cec_otp_msg));
					memcpy(presume->cec_as_msg,
					       resume_data.cec_as_msg,
					       sizeof(resume_data.cec_as_msg));
				}
			}
	}
		__switch_back_securemb();
	}
}

void set_wakeup_method(unsigned int method)
{
	resume_data.method = method;
}

void set_cec_val1(unsigned int cec_val)
{
	resume_data.date1 = cec_val;
	dbg_print("cec1: ", resume_data.date1);
}

void set_cec_val2(unsigned int cec_val)
{
	resume_data.date2 = cec_val;
	dbg_print("cec2: ", resume_data.date2);
}

void set_cec_wk_msg(unsigned char msg_idx, unsigned char *cec_wk_msg)
{
	int i = 0;

	if (!cec_wk_msg)
		return;

	switch (msg_idx) {
	case CEC_OC_IMAGE_VIEW_ON:
		memcpy(resume_data.cec_otp_msg, cec_wk_msg, cec_wk_msg[0] + 1);
		dbg_print("otp msg len: ", resume_data.cec_otp_msg[0]);
		dbg_prints("wk otp msg data:\n");
		for (i = 0; i < resume_data.cec_otp_msg[0]; i++)
			dbg_print("val:", resume_data.cec_otp_msg[i + 1]);
		break;
	case CEC_OC_ACTIVE_SOURCE:
		memcpy(resume_data.cec_as_msg, cec_wk_msg, cec_wk_msg[0] + 1);
		dbg_print("as msg len: ", resume_data.cec_as_msg[0]);
		dbg_prints("wk as msg data:\n");
		for (i = 0; i < resume_data.cec_as_msg[0]; i++)
			dbg_print("val:", resume_data.cec_as_msg[i + 1]);
		break;
	default:
		break;
	}
}

void process_high_task(unsigned command)
{
	return;
	/*unsigned *pcommand =
	    (unsigned *)(&(high_task_share_mem[TASK_COMMAND_OFFSET]));*/
}

void high_task(void)
{
	unsigned *pcommand =
	    (unsigned *)(&(high_task_share_mem[TASK_COMMAND_OFFSET]));
	unsigned *response =
	    (unsigned *)(&(high_task_share_mem[TASK_RESPONSE_OFFSET]));
	unsigned command;

	dbg_prints("high task start!\n");
	*pcommand = 0;

	while (1) {
		/* do high task process */
		command = *pcommand;
		if (command) {
			/*dbg_print("process command ", command);*/
			process_high_task(command);
			*pcommand = 0;
			*response = 0;
		}
		__switch_back_highmb();
	}
}

extern unsigned int usr_pwr_key;
extern void cec_update_config_data(void *data, unsigned int size);
extern void cec_update_phyaddress(unsigned int phyaddr);
void process_low_task(unsigned command)
{
	unsigned *pcommand =
	    (unsigned *)(&(low_task_share_mem[TASK_COMMAND_OFFSET]));
	/*unsigned *response =
	    (unsigned *)(&(low_task_share_mem[TASK_RESPONSE_OFFSET]));*/
	unsigned int cl_id = (command >> 16);
	unsigned int size = *(pcommand + 1);
	if ((command & 0xffff) == LOW_TASK_USR_DATA) {/*0-15bit: comd; 16-31bit: client_id*/
		if ((command >> 16) == SCPI_CL_REMOTE) {
			usr_pwr_key = *(pcommand + 2);/*tx_size locates at *(pcommand + 1)*/
			dbg_print("pwr_key=",usr_pwr_key);
		} else if (cl_id == SCPI_CL_SET_CEC_DATA) {
			cec_update_config_data((pcommand + 2), size);
		} else if (cl_id == SCPI_CL_UPDATE_PHYADDR) {
			cec_update_phyaddress(*(pcommand + 2));
		}
	}
}

void low_task(void)
{
	unsigned *pcommand =
	    (unsigned *)(&(low_task_share_mem[TASK_COMMAND_OFFSET]));
	unsigned *response =
	    (unsigned *)(&(low_task_share_mem[TASK_RESPONSE_OFFSET]));
	unsigned command;

	*pcommand = 0;
	dbg_prints("low task start!\n");

	while (1) {
		/* do low task process */
		command = *pcommand;
		if (command) {
			process_low_task(command);

			*pcommand = 0;
			*response = 0;
		}
		__switch_back_lowmb();
	}
}
