#ifndef __RPC_USER_H__
#define __RPC_USER_H__

/*only support max num cmd*/
#define MAX_ENTRY_NUM		128
/*Define Message Type Here*/
/*******************************************************************************
 * Message composition
 ******************************************************************************/

/* ...Message composition with module(6bits), function(10bits) */
#define __MBX_COMPOSE_MSG(mod, func)	(((mod) << 10) | ((func) & 0x3FF))

/* ...accessors */
#define MBX_MSG_MOD(msgcode)	(((msgcode) & 0x3F) >> 10)
#define MBX_MSG_FUNC(msgcode)	((msgcode) & (0x3FF))

/*******************************************************************************
 * Define module type here, 6bits valid
 ******************************************************************************/
#define MBX_SYSTEM		0x0

/*******************************************************************************
 * Define function here, 10bits valid
 ******************************************************************************/
	 /*SYSTEM*/
#define CMD_UNDEFINE            0x0
#define CMD_SUSPEND             0x4
#define CMD_SYSTEM_POWER_STATE  0x8
#define CMD_CPU_FSM_IDLE   0x9
#define CMD_SET_RTC             0x11
#define CMD_GET_RTC             0x12
#define CMD_SET_ETHERNET_WOL	0x35
#define CMD_RPCUINTREE_TEST     0x6
#define CMD_RPCUINTTEE_TEST     0x7
#define CMD_GET_CEC_INFO1	0xB4
#define CMD_GET_CEC_INFO2	0xB5
#define CMD_SET_CEC_DATA	0xB6
#define SCPI_CMD_GET_CEC_OTP_MSG	0xBB
#define SCPI_CMD_GET_CEC_AS_MSG	0xBC
#define CMD_GET_WAKEUP_REASON   0x30
#define CMD_CLR_WAKEUP_REASON   0x31
#define CMD_GET_STICK_REBOOT_FLAG  0x40
#define CMD_GET_LED_INFO        0xF7
#define CMD_VAD_AWE_WAKEUP      0x62
#define CMD_SET_KEYPAD          0xF6
#define CMD_GET_IR_INFO		0xF8
#define CMD_PM_FREEZE		0xf9



/*******************************************************************************
 * Mssage Composition
 ******************************************************************************/
#define MBX_CMD_RPCUINTREE_TEST	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_RPCUINTREE_TEST)
#define MBX_CMD_RPCUINTTEE_TEST	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_RPCUINTTEE_TEST)
#define MBX_CMD_SUSPEND	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_SUSPEND)
#define MBX_CMD_SET_RTC	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_SET_RTC)
#define MBX_CMD_GET_RTC	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_GET_RTC)
#define MBX_CMD_GET_CEC_INFO1 __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_GET_CEC_INFO1)
#define MBX_CMD_GET_CEC_INFO2 __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_GET_CEC_INFO2)
#define MBX_CMD_SET_CEC_DATA __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_SET_CEC_DATA)
#define MBX_CMD_GET_WAKEUP_OTP_MSG	__MBX_COMPOSE_MSG(MBX_SYSTEM, SCPI_CMD_GET_CEC_OTP_MSG)
#define MBX_CMD_CLR_WAKEUP_AS_MSG	__MBX_COMPOSE_MSG(MBX_SYSTEM, SCPI_CMD_GET_CEC_AS_MSG)
#define MBX_CMD_GET_WAKEUP_REASON	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_GET_WAKEUP_REASON)
#define MBX_CMD_CLR_WAKEUP_REASON	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_CLR_WAKEUP_REASON)
#define MBX_CMD_CPU_FSM_IDLE   __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_CPU_FSM_IDLE)
#define MBX_CMD_GET_LED_INFO __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_GET_LED_INFO)
#define MBX_CMD_VAD_AWE_WAKEUP __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_VAD_AWE_WAKEUP)
#define MBX_CMD_PM_FREEZE __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_PM_FREEZE)
#define MBX_CMD_SET_KEYPAD	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_SET_KEYPAD)
#define MBX_CMD_GET_IR_INFO __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_GET_IR_INFO)
#define MBX_CMD_GET_STICK_REBOOT_FLAG __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_GET_STICK_REBOOT_FLAG)
#define MBX_CMD_SET_ETHERNET_WOL	__MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_SET_ETHERNET_WOL)
#endif
