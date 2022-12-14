#ifndef __MAILBOX_H__
#define __MAILBOX_H__
#include "soc.h"
#include "register.h"

#define MHU_MAX_SIZE		0x80 /*128 char for match fifo mailbox*/
#define MHU_TASKID_SIZE		0x8
#define MHU_COPETE_SIZE		0x8
#define MHU_ULLCTL_SIZE		0x8
#define MHU_STS_SIZE		0x4
#define MHU_HEAD_SIZE		0x1c /*include status 0x4 task id 0x8, completion 0x8*/
#define MHU_RESEV_SIZE		0x4 /*include status 0x2 task id 0x8, completion 0x8*/
#define MHU_DATA_SIZE		(MHU_MAX_SIZE - MHU_HEAD_SIZE - MHU_RESEV_SIZE)
#define MHU_DATA_OFFSET		(MHU_HEAD_SIZE / 4) /*include status 0x4 task id 0x8, completion 0x8*/

#define MAILBOX_AOCPU_REE_IRQ		90//MBOX_IRQ3
#define MAILBOX_AOCPU_REEACK_IRQ	85//MBOX_IRQ2
#define MAILBOX_AOCPU_TEE_IRQ		88 //MBOX_IRQ1
#define MAILBOX_AOCPU_TEEACK_IRQ	83 //MBOX_IRQ1

#define MAILBOX_ARMREE2AO	0x3		/*mailbox3*/
#define MAILBOX_AO2ARMREE	0x2		/*mailbox2*/
#define MAILBOX_ARMTEE2AO	0x1		/*mailbox1*/
#define MAILBOX_AO2ARMTEE	0x0		/*mailbox0*/

#define MAILBOX_BASE		0xff63c400
#define MAILBOX_STAT(MBOX)	MAILBOX_BASE + ((0x2 + (0x3 * (MBOX))) << 2)  /*mailbox4 rev*/
#define MAILBOX_CLR(MBOX)	MAILBOX_BASE + ((0x3 + (0x3 * (MBOX))) << 2) /*mailbox4 rev*/
#define MAILBOX_SET(MBOX)	MAILBOX_BASE + ((0x1 + (0x3 * (MBOX))) << 2) /*mailbox4 send*/

#define MAILBOX_AO2REE	0xFFFC7400
#define MAILBOX_REE2AO	0xFFFC7600
#define MAILBOX_AO2TEE	0xFFFC7A00
#define MAILBOX_TEE2AO	0xFFFC7800
#define SIZE_SHIFT	20	/* Bit position for size value in MHU header */
#define SIZE_MASK	0x1ff	/* Mask to extract size value in MHU header*/
#endif
