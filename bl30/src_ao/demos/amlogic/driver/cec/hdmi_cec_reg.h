
/*
 * arch/arm/cpu/armv8/txl/firmware/scp_task/cec_tx_reg.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
*/

#ifndef _CEC_TX_REG_H
#define _CEC_TX_REG_H

#ifndef CONFIG_CEC_OSD_NAME
#define CONFIG_CEC_OSD_NAME "AML_TV"
#endif

/*#define CEC_VERSION "freeRTOS cec:2020/0923"*/

enum cec_reg_idx {
	CECA_REG_CLK_CNTL_REG0 = 0,
	CECA_REG_CLK_CNTL_REG1,	/*1*/
	CECA_REG_GEN_CNTL,	/*2*/
	CECA_REG_RW_REG,
	CECA_REG_INTR_MASKN,
	CECA_REG_INTR_CLR,
	CECA_REG_INTR_STAT,

	CECB_REG_CLK_CNTL_REG0,	/*7*/
	CECB_REG_CLK_CNTL_REG1,	/*8*/
	CECB_REG_GEN_CNTL,	/*9*/
	CECB_REG_RW_REG, 	/*0xa*/
	CECB_REG_INTR_MASKN,	/*0xb*/
	CECB_REG_INTR_CLR,	/*0xc*/
	CECB_REG_INTR_STAT,	/*0xd*/

	CEC_REG_STS0,
	CEC_REG_STS1,

	CEC_REG_STICK_DATA0,	/*config*/
	CEC_REG_STICK_DATA1,	/*port info*/
	CEC_REG_STICK_DATA2,
	CEC_REG_STICK_DATA3,
	CEC_REG_STICK_DATA4,
	CEC_REG_STICK_DATA5,
	CEC_REG_STICK_DATA6,
	CEC_REG_STICK_DATA7,

	CEC_REG_DEF_END
};

#define REG_MASK_ADDR	0x00ffffff
//#define REG_MASK_PR	0x01000000/*periphs register*/

/* FOR AO_CECB */
#define DWC_CECB_CTRL                0x00
#define DWC_CECB_CTRL2               0x01/*tl1 later*/
#define DWC_CECB_INTR_MASK           0x02
#define DWC_CECB_LADD_LOW            0x05
#define DWC_CECB_LADD_HIGH           0x06
#define DWC_CECB_TX_CNT              0x07
#define DWC_CECB_RX_CNT              0x08
#define DWC_CECB_STAT0               0x09/*tl1 later*/
#define DWC_CECB_TX_DATA00           0x10
#define DWC_CECB_TX_DATA01           0x11
#define DWC_CECB_TX_DATA02           0x12
#define DWC_CECB_TX_DATA03           0x13
#define DWC_CECB_TX_DATA04           0x14
#define DWC_CECB_TX_DATA05           0x15
#define DWC_CECB_TX_DATA06           0x16
#define DWC_CECB_TX_DATA07           0x17
#define DWC_CECB_TX_DATA08           0x18
#define DWC_CECB_TX_DATA09           0x19
#define DWC_CECB_TX_DATA10           0x1A
#define DWC_CECB_TX_DATA11           0x1B
#define DWC_CECB_TX_DATA12           0x1C
#define DWC_CECB_TX_DATA13           0x1D
#define DWC_CECB_TX_DATA14           0x1E
#define DWC_CECB_TX_DATA15           0x1F
#define DWC_CECB_RX_DATA00           0x20
#define DWC_CECB_RX_DATA01           0x21
#define DWC_CECB_RX_DATA02           0x22
#define DWC_CECB_RX_DATA03           0x23
#define DWC_CECB_RX_DATA04           0x24
#define DWC_CECB_RX_DATA05           0x25
#define DWC_CECB_RX_DATA06           0x26
#define DWC_CECB_RX_DATA07           0x27
#define DWC_CECB_RX_DATA08           0x28
#define DWC_CECB_RX_DATA09           0x29
#define DWC_CECB_RX_DATA10           0x2A
#define DWC_CECB_RX_DATA11           0x2B
#define DWC_CECB_RX_DATA12           0x2C
#define DWC_CECB_RX_DATA13           0x2D
#define DWC_CECB_RX_DATA14           0x2E
#define DWC_CECB_RX_DATA15           0x2F
#define DWC_CECB_LOCK_BUF            0x30
#define DWC_CECB_WAKEUPCTRL          0x31

/* cec irq bit flags for AO_CEC_B */
#define CECB_IRQ_TX_DONE		(1 << 0)
#define CECB_IRQ_RX_EOM			(1 << 1)
#define CECB_IRQ_TX_NACK		(1 << 2)
#define CECB_IRQ_TX_ARB_LOST		(1 << 3)
#define CECB_IRQ_TX_ERR_INITIATOR	(1 << 4)
#define CECB_IRQ_RX_ERR_FOLLOWER	(1 << 5)
#define CECB_IRQ_RX_WAKEUP		(1 << 6)
#define CECB_IRQ_EN_MASK		(0x3f << 0)

// tx_msg_status definition
#define TX_IDLE                 0  // No transaction
#define TX_BUSY                 1  // Transmitter is busy
#define TX_DONE                 2  // Message has been successfully transmitted
#define TX_ERROR                3  // Message has been transmitted with error

/*
 * AOCEC_A internal register
 * read/write tx register list
 */
#define CECA_TX_MSG_0_HEADER        0x00
#define CECA_TX_MSG_1_OPCODE        0x01
#define CECA_TX_MSG_2_OP1           0x02
#define CECA_TX_MSG_3_OP2           0x03
#define CECA_TX_MSG_4_OP3           0x04
#define CECA_TX_MSG_5_OP4           0x05
#define CECA_TX_MSG_6_OP5           0x06
#define CECA_TX_MSG_7_OP6           0x07
#define CECA_TX_MSG_8_OP7           0x08
#define CECA_TX_MSG_9_OP8           0x09
#define CECA_TX_MSG_A_OP9           0x0A
#define CECA_TX_MSG_B_OP10          0x0B
#define CECA_TX_MSG_C_OP11          0x0C
#define CECA_TX_MSG_D_OP12          0x0D
#define CECA_TX_MSG_E_OP13          0x0E
#define CECA_TX_MSG_F_OP14          0x0F
#define CECA_TX_MSG_LENGTH          0x10
#define CECA_TX_MSG_CMD             0x11
#define CECA_TX_WRITE_BUF           0x12
#define CECA_TX_CLEAR_BUF           0x13
#define CECA_RX_MSG_CMD             0x14
#define CECA_RX_CLEAR_BUF           0x15
#define CECA_LOGICAL_ADDR0          0x16
#define CECA_LOGICAL_ADDR1          0x17/*ADDR L tm2 later*/
#define CECA_LOGICAL_ADDR2          0x18/*ADDR H tm2 later*/
#define CECA_LOGICAL_ADDR3          0x19
#define CECA_LOGICAL_ADDR4          0x1A
#define CECA_CLOCK_DIV_H            0x1B
#define CECA_CLOCK_DIV_L            0x1C

/*
 * AOCEC_A internal register
 * read only register list
 */
#define CECA_RX_MSG_0_HEADER        0x80
#define CECA_RX_MSG_1_OPCODE        0x81
#define CECA_RX_MSG_2_OP1           0x82
#define CECA_RX_MSG_3_OP2           0x83
#define CECA_RX_MSG_4_OP3           0x84
#define CECA_RX_MSG_5_OP4           0x85
#define CECA_RX_MSG_6_OP5           0x86
#define CECA_RX_MSG_7_OP6           0x87
#define CECA_RX_MSG_8_OP7           0x88
#define CECA_RX_MSG_9_OP8           0x89
#define CECA_RX_MSG_A_OP9           0x8A
#define CECA_RX_MSG_B_OP10          0x8B
#define CECA_RX_MSG_C_OP11          0x8C
#define CECA_RX_MSG_D_OP12          0x8D
#define CECA_RX_MSG_E_OP13          0x8E
#define CECA_RX_MSG_F_OP14          0x8F
#define CECA_RX_MSG_LENGTH          0x90
#define CECA_RX_MSG_STATUS          0x91
#define CECA_RX_NUM_MSG             0x92
#define CECA_TX_MSG_STATUS          0x93
#define CECA_TX_NUM_MSG             0x94
/*
 * AOCEC_A internal register
 * read only (tl1 later)
 */
#define CECA_STAT_0_0				0xA0
#define CECA_STAT_0_1				0xA1
#define CECA_STAT_0_2				0xA2
#define CECA_STAT_0_3				0xA3
#define CECA_STAT_1_0				0xA4
#define CECA_STAT_1_1				0xA5
#define CECA_STAT_1_2				0xA6


// tx_msg_cmd definition
#define TX_NO_OP                0  // No transaction
#define TX_REQ_CURRENT          1  // Transmit earliest message in buffer
#define TX_ABORT                2  // Abort transmitting earliest message
#define TX_REQ_NEXT             3  // Overwrite earliest message in buffer and transmit next message

// tx_msg_status definition
#define TX_IDLE                 0  // No transaction
#define TX_BUSY                 1  // Transmitter is busy
#define TX_DONE                 2  // Message has been successfully transmitted
#define TX_ERROR                3  // Message has been transmitted with error

// rx_msg_cmd
#define RX_NO_OP                0  // No transaction
#define RX_ACK_CURRENT          1  // Read earliest message in buffer
#define RX_DISABLE              2  // Disable receiving latest message
#define RX_ACK_NEXT             3  // Clear earliest message from buffer and read next message

// rx_msg_status
#define RX_IDLE                 0  // No transaction
#define RX_BUSY                 1  // Receiver is busy
#define RX_DONE                 2  // Message has been received successfully
#define RX_ERROR                3  // Message has been received with error

#define CECA_HOST_INT	0x1
#define CECA_TX_INT	0x2
#define CECA_RX_INT	0x4

/* wakeup mask */
#define WAKEUP_OP_86_EN			BIT(7)
#define WAKEUP_OP_82_EN			BIT(6)
#define WAKEUP_OP_70_EN			BIT(5)
#define WAKEUP_OP_44_EN			BIT(4)
#define WAKEUP_OP_42_EN			BIT(3)
#define WAKEUP_OP_41_EN			BIT(2)
#define WAKEUP_OP_0D_EN			BIT(1)
#define WAKEUP_OP_04_EN			BIT(0)
#define WAKEUP_DIS_MASK			0
#define WAKEUP_EN_MASK		(WAKEUP_OP_86_EN | \
							WAKEUP_OP_0D_EN | \
							WAKEUP_OP_04_EN)

/*
 * CEC OPCODES
 */
#define	CEC_OC_ABORT_MESSAGE 					0xFF
#define	CEC_OC_ACTIVE_SOURCE 					0x82
#define	CEC_OC_CEC_VERSION 					0x9E
#define	CEC_OC_CLEAR_ANALOGUE_TIMER 				0x33
#define	CEC_OC_CLEAR_DIGITAL_TIMER 				0x99
#define	CEC_OC_CLEAR_EXTERNAL_TIMER 				0xA1
#define	CEC_OC_DECK_CONTROL 					0x42
#define	CEC_OC_DECK_STATUS 					0x1B
#define	CEC_OC_DEVICE_VENDOR_ID 				0x87
#define	CEC_OC_FEATURE_ABORT 					0x00
#define	CEC_OC_GET_CEC_VERSION 					0x9F
#define	CEC_OC_GET_MENU_LANGUAGE 				0x91
#define	CEC_OC_GIVE_AUDIO_STATUS 				0x71
#define	CEC_OC_GIVE_DECK_STATUS 				0x1A
#define	CEC_OC_GIVE_DEVICE_POWER_STATUS 			0x8F
#define	CEC_OC_GIVE_DEVICE_VENDOR_ID 				0x8C
#define	CEC_OC_GIVE_OSD_NAME 					0x46
#define	CEC_OC_GIVE_PHYSICAL_ADDRESS 				0x83
#define	CEC_OC_GIVE_SYSTEM_AUDIO_MODE_STATUS 			0x7D
#define	CEC_OC_GIVE_TUNER_DEVICE_STATUS 			0x08
#define	CEC_OC_IMAGE_VIEW_ON 					0x04
#define	CEC_OC_INACTIVE_SOURCE 					0x9D
#define	CEC_OC_MENU_REQUEST 					0x8D
#define	CEC_OC_MENU_STATUS 					0x8E
#define	CEC_OC_PLAY 						0x41
#define	CEC_OC_POLLING_MESSAGE 					0xFC	/* Fake Code - <Poll Message> has no OP Code and requires only the header byte */
#define	CEC_OC_RECORD_OFF 					0x0B
#define	CEC_OC_RECORD_ON 					0x09
#define	CEC_OC_RECORD_STATUS 					0x0A
#define	CEC_OC_RECORD_TV_SCREEN 				0x0F
#define	CEC_OC_REPORT_AUDIO_STATUS 				0x7A
#define	CEC_OC_REPORT_PHYSICAL_ADDRESS 				0x84
#define	CEC_OC_REPORT_POWER_STATUS 				0x90
#define	CEC_OC_REQUEST_ACTIVE_SOURCE 				0x85
#define	CEC_OC_ROUTING_CHANGE 					0x80
#define	CEC_OC_ROUTING_INFORMATION 				0x81
#define	CEC_OC_SELECT_ANALOGUE_SERVICE 				0x92
#define	CEC_OC_SELECT_DIGITAL_SERVICE 				0x93
#define	CEC_OC_SET_ANALOGUE_TIMER 				0x34
#define	CEC_OC_SET_AUDIO_RATE 					0x9A
#define	CEC_OC_SET_DIGITAL_TIMER 				0x97
#define	CEC_OC_SET_EXTERNAL_TIMER 				0xA2
#define	CEC_OC_SET_MENU_LANGUAGE 				0x32
#define	CEC_OC_SET_OSD_NAME 					0x47
#define	CEC_OC_SET_OSD_STRING 					0x64
#define	CEC_OC_SET_STREAM_PATH 					0x86
#define	CEC_OC_SET_SYSTEM_AUDIO_MODE 				0x72
#define	CEC_OC_SET_TIMER_PROGRAM_TITLE 				0x67
#define	CEC_OC_STANDBY 						0x36
#define	CEC_OC_SYSTEM_AUDIO_MODE_REQUEST 			0x70
#define	CEC_OC_SYSTEM_AUDIO_MODE_STATUS 			0x7E
#define	CEC_OC_TEXT_VIEW_ON 					0x0D
#define	CEC_OC_TIMER_CLEARED_STATUS 				0x43
#define	CEC_OC_TIMER_STATUS 					0x35
#define	CEC_OC_TUNER_DEVICE_STATUS 				0x07
#define	CEC_OC_TUNER_STEP_DECREMENT 				0x06
#define	CEC_OC_TUNER_STEP_INCREMENT 				0x05
#define	CEC_OC_USER_CONTROL_PRESSED 				0x44
#define	CEC_OC_USER_CONTROL_RELEASED 				0x45
#define	CEC_OC_VENDOR_COMMAND 					0x89
#define	CEC_OC_VENDOR_COMMAND_WITH_ID 				0xA0
#define	CEC_OC_VENDOR_REMOTE_BUTTON_DOWN 			0x8A
#define	CEC_OC_VENDOR_REMOTE_BUTTON_UP 				0x8B

/*CEC UI MASK*/
#define CEC_FUNC_MASK                        0
#define ONE_TOUCH_PLAY_MASK                  1
#define ONE_TOUCH_STANDBY_MASK               2
#define AUTO_POWER_ON_MASK                   3

//#define P_HHI_GCLK_MPEG2 CBUS_REG_ADDR(HHI_GCLK_MPEG2)
//#define P_HHI_HDMI_CLK_CNTL CBUS_REG_ADDR(HHI_HDMI_CLK_CNTL)
#define MAX_MSG 16
#define CEC_PLAYBACK_DEVICE_TYPE 4
#define CEC_BROADCAST_ADDR 0xf
#define CEC_VERSION_14A 5

enum _cec_log_dev_addr_e {
    CEC_TV_ADDR = 0x00,
    CEC_RECORDING_DEVICE_1_ADDR,
    CEC_RECORDING_DEVICE_2_ADDR,
    CEC_TUNER_1_ADDR,
    CEC_PLAYBACK_DEVICE_1_ADDR,
    CEC_AUDIO_SYSTEM_ADDR,
    CEC_TUNER_2_ADDR,
    CEC_TUNER_3_ADDR,
    CEC_PLAYBACK_DEVICE_2_ADDR,
    CEC_RECORDING_DEVICE_3_ADDR,
    CEC_TUNER_4_ADDR,
    CEC_PLAYBACK_DEVICE_3_ADDR,
    CEC_RESERVED_1_ADDR,
    CEC_RESERVED_2_ADDR,
    CEC_FREE_USE_ADDR,
    CEC_UNREGISTERED_ADDR
};

typedef enum  {
    CEC_UNRECONIZED_OPCODE = 0x0,
    CEC_NOT_CORRECT_MODE_TO_RESPOND,
    CEC_CANNOT_PROVIDE_SOURCE,
    CEC_INVALID_OPERAND,
    CEC_REFUSED,
    CEC_UNABLE_TO_DETERMINE,
} cec_feature_abort_e;

typedef enum {
    DEVICE_MENU_ACTIVE = 0,
    DEVICE_MENU_INACTIVE,
} cec_device_menu_state_e;

/* cec message structure */
typedef struct {
    unsigned char msg[16];
    unsigned char msg_len;
} cec_msg_buf_t;

typedef struct {
	cec_msg_buf_t buf[2];          /* message memory */
	unsigned char power_status;
	unsigned char log_addr;
	unsigned char cec_power;
	unsigned char active_source;
	unsigned char rx_write_pos;
	unsigned char rx_read_pos;
	unsigned char rx_buf_size;
	unsigned int addr_enable;
} cec_msg_t;

#endif  // _HDMI_RX_REG_H

