#ifndef __HDMI_CEC__
#define __HDMI_CEC__


#define CEC_VERSION "2022/05/28 support for multi-addr under uboot"

u32 cec_init_config(void);
u32 cec_suspend_handle(void);

/* cec chip version
 * freeRTOS after TM2 support cec function
 */
enum cec_chip_ver {
	CEC_CHIP_SC2,
	CEC_CHIP_T5,	/*same as t5d*/
	CEC_CHIP_T7,	/*from sc2*/
	CEC_CHIP_S4,
	CEC_CHIP_T3
};

#define CEC_A	0
#define CEC_B	1

/*cec API for suspend*/

#define CEC_TASK_PRI			5
#define CEC_TASK_STACK_SIZE		1024

#define CEC_CFG_FUNC_EN			0x01
#define CEC_CFG_OTP_EN			0x02
#define CEC_CFG_PW_OFF_EN		0x04
#define CEC_CFG_PW_ON_EN		0x08
#define CEC_CFG_DBG_EN			0x80000000

/*
 * when power down, create the cec task
 */
void vCEC_task(void *pvParameters);

/*
 * call cec wakup flag, return 1 need wakup
 */
u32 cec_get_wakup_flag(void);

void cec_req_irq(u32 onoff);
void cec_delay(u32 cnt);
void *cec_update_config_data(void *data);
void cec_update_phyaddress(unsigned int phyaddr);
void cec_update_func_cfg(unsigned int cfg);

void vCecCallbackInit(enum cec_chip_ver chip_mode);
#endif
